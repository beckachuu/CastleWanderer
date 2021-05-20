
                             GIỚI THIỆU GAME "CASTLE WANDERER"

Bối cảnh ở thời Trung Cổ tại châu Âu, nhân vật chính là một pháp sư lâu năm, thường lảng vảng ở những
ngôi làng gần lâu đài. Ở đó có những yêu tinh cục súc và những người lính gác với đủ loại tính cách nhưng
theo lời của các yêu tinh, lính gác có thể chia làm 2 loại: người tốt và người xấu. Hiểu đơn giản thì người tốt là những người
về phe yêu tinh và người xấu là những kẻ còn lại. Pháp sư hay người chơi có nhiệm vụ nói chuyện với lính gác để xác định
xem đó có phải lính gác xấu không và tiêu diệt. Khi boss cuối xuất hiện pháp sư cần đánh bại boss để chiến thắng.

-------------------------------------------------------------------------------------------------

+ Nguồn code tham khảo:
https://lazyfoo.net/tutorials/SDL/index.php
Em sử dụng code của các phần khởi tạo SDL, SDL_TTF, SDL_image và SDL_mixer trong hướng dẫn của lazyfoo (có sửa đổi 1 vài chỗ)
Phần code điều khiển nhân vật lên xuống trái phải em copy từ bài 26, sau đó viết thêm tính năng nhảy và bắn cầu lửa cho nhân vật
Những phần code em tự viết hoàn toàn:
- Điều khiển nhân vật NPC:
  + Cho lính gác, yêu tinh di chuyển ngẫu nhiên
  + Cho lính gác nói chuyện sau khi chạm vào nhân vật chính hoặc sau khi bị tấn công
  + Cho yêu tinh nói chuyện sau khi bị tấn công
  + Cho lính gác và yêu tinh đuổi theo gây sát thương cho nhân vật chính
- Cuộn màn hình
- Check va chạm giữa: nhân vật chính và lính gác/yêu tinh, cầu lửa và lính gác/yêu tinh/boss cuối

-------------------------------------------------------------------------------------------------

+ Đồ họa:
Nhân vật: https://blog.indiumgames.fi/
Background và các màn hình start, pause, end: vẽ trên app IbisPaint

-------------------------------------------------------------------------------------------------

+ Âm nhạc:
Mở đầu game: Into a Mystical Forest (by Ean Grimm: https://youtu.be/DIi-pBpXIbE)
Nhạc nền trong game: Black Wolf's Inn (by Derek Fiechter: https://youtu.be/LtDMBtjyq9A)

-------------------------------------------------------------------------------------------------

+ Cách chơi:
- Dùng các phím W,A,S,D để điều khiển nhân vật di chuyển lên, trái, xuống, phải
- Mỗi khi nhân vật di chuyển chạm vào lính gác, lính gác sẽ nói với người chơi một câu, người chơi cần dựa vào đó để phán đoán xem
lính gác là người tốt hay xấu
  Người chơi có 3 lượt chạm để đọc tính cách lính gác, sau lần chạm thứ 4 lính gác sẽ nổi giận và đuổi theo tấn công người chơi
  Thỉnh thoảng các yêu tinh sẽ đưa ra lời khuyên để nhận biết lính gác đó tốt hay xấu
- Người chơi có thể tấn công bằng phím F (bắn ra cầu lửa):
  Nếu người chơi tấn công yêu tinh, yêu tinh đó sẽ đuổi theo tới gần người chơi và phát nổ, gây sát thương rất lớn
  Người chơi tiêu diệt được lính gác xấu sẽ được cộng thêm 1 lượng máu nhất định
  Mỗi lần người chơi tiêu diệt 2 lính gác tốt, một số yêu tinh sẽ nổi giận "truy sát" người chơi (sát thương của 1 yêu tinh gây ra có thể
giảm tới 50% máu nên khả năng game kết thúc sẽ rất cao trừ khi người chơi có thể tiêu diệt đủ số lượng lính gác xấu để bù lại máu)
  Nếu người chơi tiêu diệt được 4 lính gác xấu, boss cuối cùng sẽ xuất hiện:
- Boss có 2 trạng thái: khi ở xa người chơi sẽ tự hồi máu, còn khi ở gần sẽ tấn công người chơi
- Sau khoảng thời gian nhất định boss sẽ dịch chuyển đến vị trí người chơi và tấn công bất ngờ
=> Tiêu diệt được boss sẽ thắng trò chơi

-------------------------------------------------------------------------------------------------
